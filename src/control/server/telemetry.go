//
// (C) Copyright 2018-2021 Intel Corporation.
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

package server

import (
	"context"
	"fmt"
	"net/http"

	"github.com/prometheus/client_golang/prometheus"
	"github.com/prometheus/client_golang/prometheus/promhttp"

	"github.com/daos-stack/daos/src/control/lib/telemetry/promexp"
	"github.com/daos-stack/daos/src/control/logging"
	"github.com/daos-stack/daos/src/control/server/config"
)

func regPromEngineSources(ctx context.Context, log logging.Logger, ioEngineCount int) error {
	if ioEngineCount == 0 {
		return nil
	}

	sources := make([]*promexp.EngineSource, ioEngineCount)
	for i := 0; i < ioEngineCount; i++ {
		es, err := promexp.NewEngineSource(ctx, uint32(i))
		if err != nil {
			panic(err)
		}
		sources[i] = es
	}

	opts := &promexp.CollectorOpts{
		Ignores: []string{
			`.*_ID_(\d+)_rank`,
		},
	}
	c, err := promexp.NewCollector(log, opts, sources...)
	if err != nil {
		return err
	}
	prometheus.MustRegister(c)

	return nil
}

func startPrometheusExporter(ctx context.Context, log logging.Logger, cfg *config.Server) error {
	if cfg.TelemetryPort == 0 {
		return nil
	}

	if err := regPromEngineSources(ctx, log, len(cfg.Engines)); err != nil {
		return err
	}

	http.Handle("/metrics", promhttp.HandlerFor(
		prometheus.DefaultGatherer, promhttp.HandlerOpts{},
	))
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		num, err := w.Write([]byte(`<html>
				<head><title>DAOS Exporter</title></head>
				<body>
				<h1>DAOS Exporter</h1>
				<p><a href="/metrics">Metrics</a></p>
				</body>
				</html>`))
		if err != nil {
			log.Errorf("%d: %s", num, err)
		}
	})

	listenAddress := fmt.Sprintf("0.0.0.0:%d", cfg.TelemetryPort)
	log.Infof("Listening on %s", listenAddress)

	if err := http.ListenAndServe(listenAddress, nil); err != nil {
		return err
	}

	return nil
}

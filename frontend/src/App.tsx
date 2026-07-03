import React from 'react';
import './App.css';
import { useAethericsTelemetry } from './useAethericsTelemetry';
import { RadarView, ThermalView, SystemHealth } from './DashboardComponents';

const App: React.FC = () => {
  const { data, status } = useAethericsTelemetry('ws://api.aetherics.local/ws/demo', 'http://api.aetherics.local/status');

  return (
    <div className="app-container">
      <header>
        <h1>Aetherics Monitoring</h1>
      </header>
      <main className="workspace-canvas">
        <RadarView data={data?.radar} />
        <ThermalView data={data?.thermal} />
        <SystemHealth status={status} />
      </main>
    </div>
  );
};

export default App;

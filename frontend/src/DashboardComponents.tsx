import React from 'react';
import './DashboardComponents.css';

export const RadarView: React.FC<{ data: any }> = ({ data }) => (
  <div className="card radar-card">
    <h3>Radar Status</h3>
    <div className="status-indicator">{data?.motion ? 'Motion Detected' : 'Clear'}</div>
  </div>
);

export const ThermalView: React.FC<{ data: any }> = ({ data }) => (
  <div className="card thermal-card">
    <h3>Thermal Status</h3>
    <div className="status-indicator">{data?.heat ? 'Heat Detected' : 'No Heat'}</div>
  </div>
);

export const SystemHealth: React.FC<{ status: string }> = ({ status }) => (
  <div className="card health-card">
    <h3>System Health</h3>
    <div className={`status-badge ${status}`}>{status}</div>
  </div>
);

// Aetherics Client Resilience Wrapper
class AethericsClient {
    constructor(wsUrl, httpUrl) {
        this.wsUrl = wsUrl;
        this.httpUrl = httpUrl;
        this.socket = null;
        this.usePolling = false;
        this.retryDelay = 1000;
    }

    connect() {
        this.socket = new WebSocket(this.wsUrl);
        
        this.socket.onopen = () => {
            console.log("WebSocket connected.");
            this.usePolling = false;
            this.retryDelay = 1000;
        };
        
        this.socket.onclose = () => {
            console.log("WebSocket closed. Falling back to HTTP polling.");
            this.usePolling = true;
            this.startPolling();
        };

        this.socket.onerror = (err) => {
            console.error("WebSocket error:", err);
            this.socket.close();
        };
    }

    async startPolling() {
        while (this.usePolling) {
            try {
                const response = await fetch(this.httpUrl);
                const data = await response.json();
                console.log("Polling update:", data);
            } catch (error) {
                console.error("Polling error:", error);
            }
            // Exponential backoff
            await new Promise(r => setTimeout(r, this.retryDelay));
            this.retryDelay = Math.min(this.retryDelay * 2, 30000);
        }
    }
}

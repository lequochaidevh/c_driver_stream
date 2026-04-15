// controlLibLive.js
// -----------------------------
// Control Panel + Live Config + Webcam
// -----------------------------

class ControlLibLive {
    constructor() {
        this.controls = {};
        this.config   = {};  // live JSON state
    }

    // -----------------------------
    // UI Controls
    // -----------------------------
    addButton(id, callback) {
        const el = document.getElementById(id);
        if (!el) return;
        el.addEventListener('click', callback);
        this.controls[id] = {type: 'button', el: el};
    }

    addSlider(id, labelId = null) {
        const el = document.getElementById(id);
        if (!el) return;
        const labelEl = labelId ? document.getElementById(labelId) : null;
        if (labelEl) labelEl.textContent = el.value;

        el.addEventListener('input', () => {
            if (labelEl) labelEl.textContent = el.value;
            this.config[id] = parseFloat(el.value);  // live update
        });

        this.controls[id] = {type: 'slider', el: el};
        this.config[id]   = parseFloat(el.value);
    }

    addSwitch(id) {
        const el = document.getElementById(id);
        if (!el) return;
        el.addEventListener('change', () => {
            this.config[id] = el.checked;  // live update
        });
        this.controls[id] = {type: 'switch', el: el};
        this.config[id]   = el.checked;
    }

    // -----------------------------
    // JSON Handling
    // -----------------------------
    saveState() {
        return {...this.config};
    }

    loadState(state) {
        for (const key in state) {
            if (this.controls[key]) {
                const ctrl  = this.controls[key];
                const value = state[key];
                switch (ctrl.type) {
                    case 'slider':
                        ctrl.el.value = value;
                        const labelEl =
                            document.querySelector(`[for="${key}"] + span`);
                        if (labelEl) labelEl.textContent = value;
                        break;
                    case 'switch':
                        ctrl.el.checked = value;
                        break;
                }
                this.config[key] = value;
            }
        }
    }

    loadJsonFromFile(inputEl) {
        return new Promise((resolve, reject) => {
            const file = inputEl.files[0];
            if (!file) return reject('No file selected');
            const reader  = new FileReader();
            reader.onload = e => {
                try {
                    const state = JSON.parse(e.target.result);
                    this.loadState(state);
                    resolve(state);
                } catch (err) {
                    reject(err);
                }
            };
            reader.readAsText(file);
        });
    }

    saveJsonToFile(filename = 'config.json') {
        const blob = new Blob(
            [JSON.stringify(this.config, null, 2)], {type: 'application/json'});
        const url  = URL.createObjectURL(blob);
        const a    = document.createElement('a');
        a.href     = url;
        a.download = filename;
        a.click();
        URL.revokeObjectURL(url);
    }

    // -----------------------------
    // Webcam helper
    // -----------------------------
    async startWebcam(videoEl, options = {}) {
        if (!videoEl) throw new Error('Video element not provided');
        const constraints = {
            video: {width: options.width || 640, height: options.height || 480}
        };
        if (options.deviceId)
            constraints.video.deviceId = {exact: options.deviceId};

        try {
            const stream =
                await navigator.mediaDevices.getUserMedia(constraints);
            videoEl.srcObject = stream;
            await videoEl.play();
            return stream;
        } catch (err) {
            console.error('Không thể mở webcam:', err);
            throw err;
        }
    }

    stopWebcam(videoEl) {
        if (videoEl && videoEl.srcObject) {
            const tracks = videoEl.srcObject.getTracks();
            tracks.forEach(track => track.stop());
            videoEl.srcObject = null;
        }
    }

    // -----------------------------
    // Display system info from JSON
    // -----------------------------
    displayInfo(containerEl, jsonObj) {
        containerEl.innerHTML = '';  // clear
        for (const key in jsonObj) {
            const div        = document.createElement('div');
            div.style.margin = '5px 0';
            div.textContent  = `${key}: ${jsonObj[key]}`;
            containerEl.appendChild(div);
        }
    }
}

// Singleton instance
const controlLibLive = new ControlLibLive();
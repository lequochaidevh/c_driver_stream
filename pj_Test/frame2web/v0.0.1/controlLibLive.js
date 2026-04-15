class ControlLibLive {
    constructor() {
        this.controls  = {};
        this.config    = {};
        this.listeners = {};
    }

    // ---------------- EVENT BUS ----------------
    on(event, cb) {
        if (!this.listeners[event]) this.listeners[event] = [];
        this.listeners[event].push(cb);
    }

    emit(event, data) {
        if (!this.listeners[event]) return;
        this.listeners[event].forEach(cb => cb(data));
    }

    // ---------------- UI ----------------
    addSlider(id, labelId = null) {
        const el      = document.getElementById(id);
        const labelEl = labelId ? document.getElementById(labelId) : null;

        if (!el) return;

        if (labelEl) labelEl.textContent = el.value;

        el.addEventListener('input', () => {
            const value = parseFloat(el.value);
            if (labelEl) labelEl.textContent = value;

            this.config[id] = value;
            this.emit('change', {id, value});
        });

        this.controls[id] = {type: 'slider', el};
        this.config[id]   = parseFloat(el.value);
    }

    addSwitch(id) {
        const el = document.getElementById(id);
        if (!el) return;

        el.addEventListener('change', () => {
            const value     = el.checked;
            this.config[id] = value;
            this.emit('change', {id, value});
        });

        this.controls[id] = {type: 'switch', el};
        this.config[id]   = el.checked;
    }

    // ---------------- STATE ----------------
    loadState(state) {
        for (const key in state) {
            if (!this.controls[key]) continue;

            const ctrl  = this.controls[key];
            const value = state[key];

            if (ctrl.type === 'slider') {
                ctrl.el.value = value;
                const label   = document.querySelector(`[for="${key}"] + span`);
                if (label) label.textContent = value;
            }

            if (ctrl.type === 'switch') {
                ctrl.el.checked = value;
            }

            this.config[key] = value;
        }
    }

    getState() {
        return {...this.config};
    }
}

const controlLibLive = new ControlLibLive();
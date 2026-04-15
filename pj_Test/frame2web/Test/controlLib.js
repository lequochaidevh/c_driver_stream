// controlLib.js
// -----------------------------
// Simple UI Control Library with JSON support
// -----------------------------

class ControlLib {
    constructor() {
        this.controls = {};
    }

    // Register a button
    addButton(id, callback) {
        const el = document.getElementById(id);
        if(!el) return;
        el.addEventListener('click', callback);
        this.controls[id] = { type: 'button', el: el };
    }

    // Register a slider
    addSlider(id, labelId=null) {
        const el = document.getElementById(id);
        if(!el) return;
        const labelEl = labelId ? document.getElementById(labelId) : null;
        if(labelEl) labelEl.textContent = el.value;
        el.addEventListener('input', () => {
            if(labelEl) labelEl.textContent = el.value;
        });
        this.controls[id] = { type: 'slider', el: el };
    }

    // Register a switch (checkbox)
    addSwitch(id) {
        const el = document.getElementById(id);
        if(!el) return;
        this.controls[id] = { type: 'switch', el: el };
    }

    // Save current UI state to JSON object
    saveState() {
        const state = {};
        for(const key in this.controls) {
            const ctrl = this.controls[key];
            switch(ctrl.type) {
                case 'slider': state[key] = parseFloat(ctrl.el.value); break;
                case 'switch': state[key] = ctrl.el.checked; break;
                case 'button': break; // buttons not saved
            }
        }
        return state;
    }

    // Load JSON state into controls
    loadState(state) {
        for(const key in state) {
            if(this.controls[key]) {
                const ctrl = this.controls[key];
                switch(ctrl.type) {
                    case 'slider': 
                        ctrl.el.value = state[key];
                        const labelEl = document.querySelector(`[for="${key}"] + span`);
                        if(labelEl) labelEl.textContent = state[key];
                        break;
                    case 'switch':
                        ctrl.el.checked = state[key];
                        break;
                }
            }
        }
    }

    // Load JSON from file input element
    loadJsonFromFile(inputEl) {
        return new Promise((resolve, reject) => {
            const file = inputEl.files[0];
            if(!file) return reject("No file selected");
            const reader = new FileReader();
            reader.onload = e => {
                try {
                    const state = JSON.parse(e.target.result);
                    this.loadState(state);
                    resolve(state);
                } catch(err) {
                    reject(err);
                }
            };
            reader.readAsText(file);
        });
    }

    // Save JSON to download
    saveJsonToFile(filename="config.json") {
        const state = this.saveState();
        const blob = new Blob([JSON.stringify(state, null, 2)], {type: "application/json"});
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = filename;
        a.click();
        URL.revokeObjectURL(url);
    }
}

// Singleton instance
const controlLib = new ControlLib();
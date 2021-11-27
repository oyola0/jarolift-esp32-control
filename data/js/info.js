import { selector } from "./utils.js";

let timeout;

const showMsg = (cls, msg) => {
    const infoCls = 'info';
    const el = selector(infoCls);
    el.innerHTML = typeof msg === 'object' ? JSON.stringify(msg) : msg;
    el.classList = `${infoCls} ${cls}`;
    
    if (timeout) {
        clearTimeout(timeout);
    }

    timeout = setTimeout(() => {
        el.classList = `${infoCls} ${cls} fadeout`;
    }, 2000);

    return msg;
};

export const error = (msg) => showMsg('error', msg);

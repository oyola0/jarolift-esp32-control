import { getChannel, selector } from "./utils.js";
import { get } from "./fetch.js";

const touchSupported = 'ontouchstart' in window;
const upEvent = touchSupported ? 'touchend' : 'mouseup';
const downEvent = touchSupported ? 'touchstart' : 'mousedown';

const vibrate = () => window.navigator.vibrate && window.navigator.vibrate(10);

export const listener = (clsSelector) => {
    const el = selector(clsSelector);
    const btnUrl = clsSelector === 'middle' ? 'middle' : `button/${clsSelector}`;

    const upCb = () => {   
        el.className = el.className.replace(/active/g, '').trim();
        vibrate();
        el.removeEventListener(upEvent, upCb);
        el.removeEventListener('mouseleave', upCb);        
    };
    
    el.addEventListener(downEvent, () => {  
        el.className = `${el.className} active`;
        vibrate();
        el.addEventListener(upEvent, upCb);
        el.addEventListener('mouseleave', upCb);
        get(`/api/${btnUrl}/channel/${getChannel()}`);
    });    
};

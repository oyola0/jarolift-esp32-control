import { getCache, setCache } from "./localstorage.js";
import { listener } from "./listener.js";
import { combo } from "./utils.js";

const setVH = () => {
    const vh = window.innerHeight * 0.01;
    document.documentElement.style.setProperty('--vh', `${vh}px`);
};

listener('up');
listener('down');
listener('stop');
listener('middle');

combo.addEventListener('change', () => setCache());
combo.value = getCache();

window.addEventListener("resize", setVH);
setVH();

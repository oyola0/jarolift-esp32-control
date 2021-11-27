import { getChannel } from "./utils.js";

const CACHE_KEY = 'previous-option';

export const setCache = () => localStorage.setItem(CACHE_KEY, getChannel());

export const getCache = () => localStorage.getItem(CACHE_KEY) || 1;

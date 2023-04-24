import Http from "../api/Http.js";

export default defineNuxtPlugin(() => {
	return {
		provide: {
			http: new Http({
				withCredentials: true,
				headers: { Accept: 'application/json' },
				// if production, use the production api url 34.151.220.144
				baseURL: process.env.NODE_ENV === "production" ? "https://erp-api.tintasantares.com.br/erp/" : "http://localhost:8000/erp/",
			}),
		},
	};
});

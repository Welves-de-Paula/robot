import { defineStore } from "pinia";

export const useAuthStore = defineStore("auth", {
	state: () => ({
		$http: useNuxtApp().$http,

		userData: null,
	}),

	getters: {
		isLoggedIn: (state) => !!state.userData,

		user: (state) => state.userData?.member,

		userName: (state) => state.userData?.member.name,

	},

	actions: {
		async boot() {
			let token = this.getTokenOnLocalStorage();

			if (token) {
				this.setHttpAuthorizationToken(token);

				await this.getUserData().catch((error) => { });
			}

			// No need to set isBooted, as it's not used in the converted code
		},

		async login(params) {
			return new Promise((resolve, reject) => {
				this.$http
					.$post("/auth/login", params)
					.then(async (response) => {
						let token = response.token;

						await this.processToken(token);

						resolve(response);
					})
					.catch((error) => {
						reject(error);
					});
			});
		},

		async logout() {
			//await this.$http.$post(this.options.urls.logout);
			this.removeTokenOnLocalStorage();
			this.removeHttpAuthorizationToken();
			this.removeUserData();
		},

		async getUserData() {
			const { $http } = useNuxtApp();

			return new Promise((resolve, reject) => {
				$http
					.$get("/auth/loggedUser")
					.then((response) => {
						this.userData = response;

						resolve(response);
					})
					.catch((error) => {
						reject(error);
					});
			});
		},

		async processToken(token) {
			this.setHttpAuthorizationToken(token);

			this.setTokenOnLocalStorage(token);

			await this.getUserData();
		},

		removeUserData() {
			this.userData = null;
		},

		getTokenOnLocalStorage() {
			return JSON.parse(localStorage.getItem('authToken'));
		},

		setTokenOnLocalStorage(token) {
			localStorage.setItem('authToken', JSON.stringify(token));
		},

		removeTokenOnLocalStorage() {
			localStorage.removeItem('authToken');
		},

		setHttpAuthorizationToken(token) {
			const { $http } = useNuxtApp();

			$http.setAuthorizationToken(token);
		},

		removeHttpAuthorizationToken() {
			const { $http } = useNuxtApp();

			$http.removeAuthorizationToken();
		},
	},
});

import Vue from 'vue'
import store from '../store'

class Auth {

  constructor(http, options) {

    this.http = http

    this.authTokenKey = "authToken";

    this.isBooted = false;

    this.options = options;
  }

  async boot() {
    let token = this.getTokenOnLocalStorage();

    if (token) {
      this.setHttpAuthorizationToken(token);

      await this.getUserData().catch((error) => { });
    }

    this.isBooted = true;
  }

  login(params) {
    return new Promise((resolve, reject) => {
      this.http
        .$post('/login', params) // todo
        .then(async (response) => {
          let token = this.options.getTokenFunction(response);

          await this.storeToken(token);

          await this.getUserData();

          resolve(response);
        })
        .catch((error) => {
          reject(error);
        });
    });
  }

  logout() {
    return new Promise((resolve, reject) => {
      Vue.$http
        .$post(this.options.urls.logout)
        .then((response) => {
          this.removeTokenOnLocalStorage();

          this.removeHttpAuthorizationToken()

          this.removeUserData()

          resolve(response);
        })
        .catch((error) => {
          console.log(error);
        });
    });
  }

  getUserData() {
    return new Promise((resolve, reject) => {
      Vue.$http
        .$get(this.options.urls.userData)
        .then((response) => {
          this.setUserData(response);

          resolve(response);
        })
        .catch((error) => {
          reject(error);
        });
    });
  }

  storeToken(token) {
    this.setTokenOnLocalStorage(token);
    this.setHttpAuthorizationToken(token);
  }

  isLoggedIn() {
    return store.getters["auth/isLoggedIn"];
  }

  userData() {
    return store.state.auth.userData;
  }

  setUserData(data) {
    store.commit("auth/setUserData", data);
  }

  removeUserData() {
    store.commit("auth/removeUserData");
  }

  getTokenOnLocalStorage() {
    return JSON.parse(localStorage.getItem(this.authTokenKey));
  }

  setTokenOnLocalStorage(token) {
    localStorage.setItem(this.authTokenKey, JSON.stringify(token));
  }

  removeTokenOnLocalStorage() {
    localStorage.removeItem(this.authTokenKey);
  }

  setHttpAuthorizationToken(token) {
    Vue.$http.setAuthorizationToken(token);
  }

  removeHttpAuthorizationToken() {
    Vue.$http.removeAuthorizationToken();
  }

  // old function
  async getLoginData() {
    return await this.getUserData()
  }

}

export default Auth;

import axios from 'axios'

import { useAppStore } from '~/store/app'

class Http {

  constructor(options) {
    this.httpClient = axios.create(options)

    this.appStore = useAppStore()
  }

  setAuthorizationToken(token) {
    this.httpClient.defaults.headers.common['Authorization'] = `Bearer ${token}`;
  }

  removeAuthorizationToken() {
    this.httpClient.defaults.headers.common['Authorization'] = null;
  }

  $get(url, params) {
    return new Promise((resolve, reject) => {
      this.httpClient.get(url, { params })
        .then((response) => {
          resolve(response.data)
        }).catch(error => {
          this.processError(error)
          reject(error)
        });
    })
  }

  $post(url, params) {
    return new Promise((resolve, reject) => {
      this.httpClient.post(url, params)
        .then((response) => {
          resolve(response.data)
        }).catch(error => {
          this.processError(error)
          reject(error)
        });
    })
  }

  $put(url, params) {
    return new Promise((resolve, reject) => {
      this.httpClient.put(url, params)
        .then((response) => {
          resolve(response.data)
        }).catch(error => {
          this.processError(error)
          reject(error)
        });
    })
  }

  $delete(url, params = null) {
    return new Promise((resolve, reject) => {
      this.httpClient.delete(url, { params })
        .then((response) => {
          resolve(response.data)
        }).catch(error => {
          this.processError(error)
          reject(error)
        });
    })
  }

  index(url, params = null) {
    return this.$get(url, params)
  }

  show(url, id, params = null) {
    return this.$get(url + '/' + id, params)
  }

  store(url, params) {
    return this.$post(url, params)
  }

  update(url, id, params) {
    return this.$put(`${url}/${id}`, params)
  }

  destroy(url, id, params = null) {
    return this.$delete(`${url}/${id}`, params)
  }

  download(url, data, config) {
    return this.httpClient.post(url, data, { responseType: "blob" })
    // let downloadApi = this.httpClient
    // console.log(this.httpClient.post);
    // this.httpClient.post(url, params)

    // return this.$delete(`${url}/${id}`)


    // url: `${process.env.VUE_APP_API_URL}transactions/generate-receipt`,
    //   method: "POST",
    //     responseType: "blob",
  }

  processError(error) {
    this.appStore.setHttpError(error)
  }
}

export default Http;
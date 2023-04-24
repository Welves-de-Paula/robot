import { defineStore } from "pinia";

export const useAppStore = defineStore("appStore", {

  state: () => ({

    httpError: null,

    isLoading: false,

  }),

  actions: {

    setHttpError(error) {
      this.httpError = error
    },
    
    setLoading(loading) {
      this.isLoading = loading
    },
    
    startLoading() {
      this.isLoading = true
    },

    endLoading() {
      this.isLoading = false
    }

  }
})
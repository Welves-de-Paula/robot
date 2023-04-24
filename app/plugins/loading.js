import { useAppStore } from '~/store/app'


export default defineNuxtPlugin(() => {
  const appStore = useAppStore()

  return {
    provide: {
      loading: {

        start() {
          appStore.startLoading()

        },

        finish() {
          appStore.endLoading()
        },

        show() {
          appStore.startLoading()
        },

        stop() {
          appStore.endLoading()
        }
      }

    },
  };
});

// // import store from '@/store'
// import Vue from 'vue';

// Vue.use({
//   install(Vue) {

// })

// export default (_) => { }
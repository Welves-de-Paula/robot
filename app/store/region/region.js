// src/store/regionsStore.js
import { defineStore } from 'pinia';

export const useRegionStore = defineStore('regionStore', {
  state: () => ({
    http: useNuxtApp().$http,
    query: {
      search: null,
      page: 1,
      orderBy: 'name',
      orderDirection: 'asc',
    },
    regions: {
      data: [],
    },

  }),

  actions: {
    resetRegions() {
      this.regions = {
        data: [],
      };
    },

    async index() {
      const data = await this.http.index('customer/region', this.query);
      this.regions = data.regions;
    },

    async show(id) {
      const data = await this.http.show('customer/region', id);
      return data.region;
    },

    async store(form) {
      const data = await this.http.store('customer/region', form);
      return data.region;
    },

    async update(form) {
      const data = await this.http.update('customer/region', form.id, form);
      return data.region;
    },
  },
});

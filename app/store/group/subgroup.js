// src/store/regionsStore.js
import { defineStore } from 'pinia';

export const useSubgroupStore = defineStore('subgroupStore', {
  state: () => ({
    http: useNuxtApp().$http,
    query: {
      search: null,
      page: 1,
      orderBy: 'name',
      orderDirection: 'asc',
    },
    subgroups: {
      data: [],
    },

  }),

  actions: {


    async index() {
      const data = await this.http.index('product/subgroup', this.query);
      this.subgroups = data.subgroups;
    },

    async show(id) {
      const data = await this.http.show('product/subgroup', id);
      return data.subgroup;
    },

    async store(form) {
      const data = await this.http.store('product/subgroup', form);
      return data.subgroup;
    },

    async update(form) {
      const data = await this.http.update('product/subgroup', form.id, form);
      return data.subgroup;
    },
  },
});

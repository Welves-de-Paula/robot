// src/store/regionsStore.js
import { defineStore } from 'pinia';

export const useGroupStore = defineStore('groupStore', {
  state: () => ({
    http: useNuxtApp().$http,
    query: {
      search: null,
      page: 1,
      orderBy: 'name',
      orderDirection: 'asc',
    },
    groups: {
      data: [],
    },

  }),

  actions: {
    resetGroups() {
      this.groups = {
        data: [],
      };
    },

    async index() {
      const data = await this.http.index('product/group', this.query);
      this.groups = data.groups;
    },

    async show(id) {
      const data = await this.http.show('product/group', id);
      return data.group;
    },

    async store(form) {
      const data = await this.http.store('product/group', form);
      return data.group;
    },

    async update(form) {
      const data = await this.http.update('product/group', form.id, form);
      return data.group;
    },
  },
});

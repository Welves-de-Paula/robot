import { defineStore } from "pinia";

export const useUserStore = defineStore("userStore", {

  state: () => ({

    http: useNuxtApp().$http,

    query: {
      search: "",
      page: 1,
      orderBy: "name",
      orderDirection: "asc",
    },

    users: {
      data: []
    },
  }),

  actions: {

    resetUsers() {
      this.users = {
        data: []
      };
    },

    async index() {

      const data = await this.http.index("user/user", this.query);

      this.users = data.users;
    },

    async show(id) {

      const data = await this.http.show("user/user", id);

      return data.user;

    },

    async store(form) {

      const data = await this.http.store("user/user", form);

      return data.user;
    },

    async update(form) {

      const data = await this.http.update("user/user", form.id, form);

      return data.user
    },
  }
});

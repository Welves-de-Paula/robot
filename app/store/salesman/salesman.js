import { defineStore } from "pinia";

export const useSalesmanStore = defineStore("salesmanStore", {

  state: () => ({

    http: useNuxtApp().$http,

    query: {
      search: "",
      page: 1,
      orderBy: "name",
      orderDirection: "asc",
    },

    salesmen: {
      data: []
    },
  }),

  actions: {

    resetUsers() {
      this.salesmen = {
        data: []
      };
    },

    async index() {

      const data = await this.http.index("salesman/salesman", this.query);

      this.salesmen = data.salesmen;
    },

    async show(id) {

      const data = await this.http.show("salesman/salesman", id);

      return data.salesman;

    },

    async store(form) {

      const data = await this.http.store("salesman/salesman", form);

      return data.salesman;
    },

    async update(form) {

      const data = await this.http.update("salesman/salesman", form.id, form);

      return data.salesman
    },
  }
});

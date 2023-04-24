import { defineStore } from "pinia";

export const useProductStore = defineStore("productStore", {

  state: () => ({

    http: useNuxtApp().$http,

    query: {
      search: "",
      page: 1,
      orderBy: "name",
      orderDirection: "asc",
    },

    products: {
      data: []
    },
  }),

  actions: {


    async index() {

      const data = await this.http.index("product/product", this.query);

      this.products = data.products;
    },

    async show(id) {

      const data = await this.http.show("product/product", id);

      return data.product;

    },

    async store(form) {

      const data = await this.http.store("product/product", form);

      return data.product;
    },

    async update(form) {

      const data = await this.http.update("product/product", form.id, form);

      return data.product
    },
  }
});

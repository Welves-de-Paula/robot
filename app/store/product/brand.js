import { defineStore } from "pinia";

export const useBrandStore = defineStore("brandStore", {

  state: () => ({

    http: useNuxtApp().$http,

    query: {
      search: "",
      page: 1,
      orderBy: "name",
      orderDirection: "asc",
    },

    brands: {
      data: []
    },

    brandForm : {
      id: null,
      code : null,
      name: null,
      status: true,
    },

  }),

  actions: {

    async index() {

      const data = await this.http.index("product/brand", this.query);

      this.brands = data.brands;
    },

    async show(id) {

      const data = await this.http.show("product/brand", id);

      return data.brand;

    },

    async store() {

      const data = await this.http.store("product/brand", this.brandForm);

      return data.brand;
    },

    async update() {

      const data = await this.http.update("product/brand", this.brandForm.id, this.brandForm);

      return data.product
    },

    async loadForm(id) {
      
      const brand = await this.show(id)

      this.brandForm = brand;
    },

    async resetForm() {
      this.brandForm = {
        id: null,
        code : null,
        name: null,
        status: true,
      };
    },

    async storeForm() {
      if (this.brandForm.id) {
        await this.update();
      } else {
        await this.store();
      }
    }
  }
});

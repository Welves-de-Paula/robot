import { defineStore } from "pinia";

import { useSalesmanStore } from "./salesman";
const userStore = useSalesmanStore();

export const useSalesmanStoreForm = defineStore("salesmanStoreForm", {

  state: () => ({

    form: {
      id: null,
      name: null,
      email: null,
      whatsapp: null,
      status: true,
      regions: null,
    },
  }),


  getters: {
    title() {
      return this.form.id ? "Editar Vendedor" : "Novo Vendedor";
    },

    isInsert() {
      return this.form.id == null;
    },
  },


  actions: {

    async show(id) {

      await userStore.show(id).then((response) => {
        this.form = response
      }).catch((error) => { });
    },

    async save() {

      if (this.isInsert) {
        await userStore.store(this.form);
      }

      if (!this.isInsert) {
        await userStore.update(this.form);
      }

    },

    reset() {
      this.form = JSON.parse(JSON.stringify(this.baseForm));
    }

  }
});

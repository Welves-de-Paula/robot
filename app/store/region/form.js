import { defineStore } from "pinia";

import { useRegionStore } from "./region";
const regionStore = useRegionStore();

export const useRegionStoreForm = defineStore("regionStoreForm", {



  state: () => ({

    form: {
      id: null,
      name: null,
      status: true,
    },


  }),


  getters: {
    title() {
      return this.form.id ? "Editar Região" : "Nova Região";
    },

    isInsert() {
      return this.form.id == null;
    },


  },


  actions: {

    async show(id) {

      await regionStore.show(id).then((response) => {
        this.form = response
      }).catch((error) => { });
    },

    async save() {

      if (this.isInsert) {
        await regionStore.store(this.form);
      }

      if (!this.isInsert) {
        await regionStore.update(this.form);
      }

    },



  }
});

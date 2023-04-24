import { defineStore } from "pinia";

import { useUserStore } from "./user";
const userStore = useUserStore();

export const useUserStoreForm = defineStore("userStoreForm", {

  state: () => ({

    form: {

      id: null,
      salesman_id: null,
      name: null,
      email: null,
      whatsapp: null,
      password: null,
      password_confirmation: null,
      status: true,
    },




  }),


  getters: {
    title() {
      return this.form.id ? "Editar Usuário" : "Novo Usuário";
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

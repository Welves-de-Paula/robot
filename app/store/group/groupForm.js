import { defineStore } from "pinia";

import { useGroupStore } from "./group";
const groupStore = useGroupStore();

export const useGroupForm = defineStore("groupForm", {

  state: () => ({
    form: {
      id: null,
      name: null,
      status: true,
    },
  }),


  getters: {
    title() {
      return this.form.id ? "Editar Grupo" : "Novo Grupo";
    },

    isInsert() {
      return this.form.id == null;
    },
  },


  actions: {

    async show(id) {

      await groupStore.show(id).then((response) => {
        this.form = response
      }).catch((error) => { });
    },

    async save() {

      if (this.isInsert) {
        await groupStore.store(this.form);
      }

      if (!this.isInsert) {
        await groupStore.update(this.form);
      }
    },
  }
});

import { defineStore } from "pinia";

import { useGroupStore } from "./group";
const groupStore = useGroupStore();


export const useSubgroupForm = defineStore("subgroupForm", {

  state: () => ({
    http: useNuxtApp().$http,

    form: {
      id: null,
      group: null,
      name: null,
      status: true,
    },
  }),


  getters: {
    title() {
      return this.form.id ? "Editar Subgrupo" : "Novo Subgrupo";
    },

    isInsert() {
      return this.form.id == null;
    },
  },


  actions: {

    async loadGroup(group_id) {
      await groupStore.show(group_id).then((response) => {

        this.form.group = response;
      }).catch((error) => { });
    },

    async show(id) {
      const data = await this.http.show('product/subgroup', id);

      this.form = data.subgroup;

    },

    async save() {
      if (this.isInsert) {
        await this.store();
      }

      if (!this.isInsert) {
        await this.update();
      }
    },



    async store() {
      console.log('store');

      const data = await this.http.store('product/subgroup', this.form);
      return data.subgroup;
    },

    async update() {
      const data = await this.http.update('product/subgroup', this.form.id, this.form);
      return data.subgroup;
    },




  }
});

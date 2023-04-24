import { defineStore } from "pinia";

import { useProductStore } from "./product";
const productStore = useProductStore();


export const useProductFormStore = defineStore("productFormStore", {

  state: () => ({

    form: {
      code: null,
      group_id: null,
      subgroup_id: null,
      brand_id: null,
      unity_id: null,
      tax_classification_id: null,
      name: null,
      notes: null,
      status: null,
      product_variants: [
        {
          name: null,
          unit: null,
          gtin: null,
          quantity_package: null,
          weight_net: null,
          weight_gross: null,
          weight_liters: null,
          weight_conversion_factor: null,
          commission: null,
          price_raw_material: null,
          price_commission: null,
          price: null,
          stock: null,
          stock_min: null,
          stock_max: null,
          stock_location: null,
          status: null,
        },
      ],
    },

  }),

  getters: {
    title() {
      return this.form.id ? "Editar Produto" : "Novo Produto";
    },



    isInsert() {
      return this.form.id == null;
    },



  },



  actions: {


    async show(id) {

      await productStore.show(id).then((response) => {
        this.form = response
      }).catch((error) => { });
    },

    async save() {

      if (this.isInsert) {
        await productStore.store(this.form);
      }

      if (!this.isInsert) {
        await productStore.update(this.form);
      }

    },

  }
});

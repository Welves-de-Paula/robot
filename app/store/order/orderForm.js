import { defineStore } from "pinia";

export const useOrderFormStore = defineStore("orderFromStore", {

  state: () => ({
    http: useNuxtApp().$http,

    order: {
      id: null,
      customer_id: null,
      order_products: [],
    },

    orderProductUpdateIndex: null,

    orderProductForm: {
      id: null,
      product: null,
      product_id: null,
      unit: null,
      quantity: 1,
      price: 0,
      product_total: 0,
      net_total: 0,
      // id: null,
      // product_id: null,
      // product_name: null,
      // quantity: null,
      // price: null,
    }
  }),

  getters: {
    total() {
      return this.order.order_products.reduce((total, product) => {
        return total + product.price * product.quantity;
      }, 0);
    },

    form() {
      return this.orderProductForm;
    }
  },

  actions: {

    removeOrderProduct(index) {
      this.order.order_products.splice(index, 1);
    },

    editOrderProduct(index) {
      this.resetOrderProductForm();

      this.orderProductUpdateIndex = index;

      this.orderProductForm = JSON.parse(JSON.stringify(this.order.order_products[index]));
    },

    storeOrderProduct() {
      if (this.orderProductUpdateIndex != null) {
        this.updateOrderProduct();
      } else {
        console.log('insert');
        this.insertOrderProduct();
      }
    },

    insertOrderProduct() {
      this.order.order_products.push(this.orderProductForm);

      this.resetOrderProductForm();
    },

    updateOrderProduct() {
      this.order.order_products[this.orderProductUpdateIndex] = this.orderProductForm;

      this.resetOrderProductForm();
    },

    resetOrderProductForm() {
      console.log('reset');
      this.orderProductUpdateIndex = null;

      this.orderProductForm = {
        id: null,
        product: null,
        product_id: null,
        unit: null,
        quantity: 1,
        price: 0,
        product_total: 0,
        net_total: 0,
      };
    },
  }
});
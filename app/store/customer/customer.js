import { defineStore } from "pinia";

export const useCustomerStore = defineStore("customerStore", {

	state: () => ({

		http: useNuxtApp().$http,

		query: {
			search: "",
			page: 1,
			orderBy: "name",
			orderDirection: "asc",
		},

		customers: {
			data: []
		},
	}),

	actions: {

		resetCustomers() {
			this.customers = {
				data: []
			};
		},

		async index() {

			const data = await this.http.index("customer/customer", this.query);

			this.customers = data.customers;
		},

		async show(id) {

			const data = await this.http.show("customer/customer", id);

			return data.customer;

		},

		async store(form) {

			const data = await this.http.store("customer/customer", form);

			return data.customer;
		},

		async update(form) {

			const data = await this.http.update("customer/customer", form.id, form);

			return data.customer
		},
	}
});

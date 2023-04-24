import { defineStore } from "pinia";

import { useCustomerStore } from "./customer";
const customerStore = useCustomerStore();

export const useCustomerStoreForm = defineStore("customerStoreForm", {

	state: () => ({

		form: {},

		baseForm: {

			code: null,
			person_type: "fisica",
			name: null,
			alias: null,
			cpf: null,
			cnpj: null,
			rg: null,
			status: 1,
			inscricao_estadual: null,

			main_address: {
				address: null,
				number: null,
				complement: null,
				district: null,
				postal_code: null,
				city_district: null,
				city: null,
				state: null,
				country: null,
				type: "mainAddress",
				region: null,
				region_id: null,
			},
			phones: [],
			emails: [],
			sites: [],
		},


	}),


	getters: {
		title() {
			return this.form.id ? "Editar Cliente" : "Novo Cliente";
		},


		IsJuridic() {
			return this.form.person_type == "juridica";
		},

		isInsert() {
			return this.form.id == null;
		},

		alias_label() {
			return this.IsJuridic ? "Nome Fantasia" : "Apelido";
		},

		name_label() {
			return this.IsJuridic ? "Razão Social" : "Nome";
		},

	},


	actions: {

		async show(id) {

			await customerStore.show(id).then((response) => {
				this.form = response
			}).catch((error) => { });
		},

		async save() {

			if (this.isInsert) {
				await customerStore.store(this.form);
			}

			if (!this.isInsert) {
				await customerStore.update(this.form);
			}

		},

		reset() {
			this.form = JSON.parse(JSON.stringify(this.baseForm));
		}

	}
});

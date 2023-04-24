<template>
  <div>
    <v-alert text v-if="customerData">
      <!-- Total a Receber: <b> {{ $format.decimal(customerData.stats.to_pay_bill_receivables_totals.sum_due_value, 2) }}</b>
      <br>
      Valor em atraso: <b> {{ $format.decimal(customerData.stats.net_overdue_value.sum_due_value, 2) }}</b> 
      <br>-->
      <template v-if="customerData.customer.last_credit_analysis">
        <template v-if="customerData.customer.last_credit_analysis.is_overdue">
          <b>Análise de crédito vencida!</b>
        </template>
        <br />
        Limite de Crédito:
        {{
          $format.decimal(
            customerData.customer.last_credit_analysis.credit_limit
          )
        }}
        <br />
        Validade:
        {{
          $format.dateBr(customerData.customer.last_credit_analysis.due_date)
        }}
      </template>
      <template v-else> Cliente sem análise de crédito </template>
    </v-alert>
  </div>
</template>

<script>
export default {
  props: {
    customer: {},
  },

  data: () => ({
    customerData: null,
  }),

  watch: {
    customer() {
      if (this.customer) {
        this.showCustomer(this.customer.id);
      } else {
        this.customerData = null;
      }
    },
  },
  methods: {
    async showCustomer(id) {
      await this.$http.show("customer/customer", id).then((response) => {
        this.customerData = response;
      });
    },
  },
};
</script>

<style>
</style>
<template>
  <div>
    {{ customer.name }}
    <!-- <app-icon
      @click.stop="handleOpenCustomerForm()"
      v-if="!hideLink"
      small
      color="info"
      >north_east
    </app-icon>
    <CustomerIcon v-if="!hideIcon" small class="mx-1" :customer="customer" />
    <small class="text--disabled">#{{ customer.code }}</small>
    <StoreCustomerForm ref="StoreCustomerForm" /> -->
  </div>
</template>

<script>
import CustomerIcon from "@/components/customer/ui/CustomerIcon.vue";
import StoreCustomerForm from "@/components/customer/forms/StoreCustomerForm.vue";

export default {
  components: { CustomerIcon, StoreCustomerForm },
  props: {
    customer: {
      type: Object,
      required: true,
    },

    hideIcon: {
      type: Boolean,
      default: false,
    },
    hideLink: {
      type: Boolean,
      default: false,
    },
  },

  methods: {
    handleOpenCustomerForm() {
      this.$refs.StoreCustomerForm.open(this.customer.id);
    },
  },
};
</script>

<style>
</style>
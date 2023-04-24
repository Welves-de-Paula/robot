<template>
  <i>
    <!-- <app-icon
      :color="status.color"
      v-for="(status, index) in statuses"
      :key="index"
      v-on="$listeners"
      v-bind="$attrs"
      v-text="status.icon"
    /> -->
  </i>
</template>

<script>
export default {
  props: {
    customer: {
      type: Object,
      default: () => ({}),
      required: true,
    },
  },

  data() {
    return {
      statusesData: [
        { type: "is_special", icon: "grade", color: "warning" },
        { type: "is_blocked", icon: "lock", color: "red" },
        { type: "is_overdue", icon: "event_busy", color: "warning" },
        // { type: "is_overdue", icon: "monetization_on", color: "primary" },
        { type: "has_debit", icon: "monetization_on", color: "red" },
      ],
    };
  },

  computed: {
    creditAnalysis() {
      return this.customer.last_credit_analysis;
    },
    is_special() {
      return this.customer.is_special;
    },

    is_blocked() {
      return this.customer.is_blocked;
    },
    is_overdue() {
      return this.customer.last_credit_analysis.is_overdue;
    },
    has_debit() {
      return this.customer.has_debit;
    },

    statuses() {
      return this.statusesData.filter((icon) => {
        return this[icon.type];
      });
    },
  },
};
</script>

<style>
</style>
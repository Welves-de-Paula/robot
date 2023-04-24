<template>
  <div>
    <template v-for="(contact, index) in contacts" :key="index">
      <v-row>
        <v-col cols="10">
          <app-text-field
            v-model="contact.contact"
            label="Email"
            @input="handleInput($event, index)"
          />
        </v-col>
        <v-col>
          <v-btn
            v-if="hasMoreThanOneRow"
            @click="removeRow(index)"
            size="small"
            variant="text"
            icon="mdi-window-close"
          />
        </v-col>
      </v-row>
    </template>
  </div>
</template>

<script>
export default {
  props: {
    modelValue: {},
  },

  data: () => ({
    contacts: [
      {
        contact: null,
        marker: null,
      },
    ],

    markers: [],
  }),

  computed: {
    hasMoreThanOneRow() {
      return this.contacts.length > 1;
    },
  },

  watch: {
    modelValue(val) {
      this.setContacts(val);
    },

    contacts: {
      handler(val) {
        this.$emit("input", val);
      },
      deep: true,
    },
  },

  created() {
    this.setContacts(this.modelValue);
  },

  methods: {
    setContacts(value) {
      this.contacts = value;
    },
    removeRow(index) {
      this.contacts.splice(index, 1);
    },

    handleInput(value, index) {
      if (this.isLastRow(index) && value !== null) {
        this.addRow();
      }
    },

    isLastRow(index) {
      return index + 1 == this.contacts.length;
    },

    addRow() {
      this.contacts.push({
        contact: null,
        marker: null,
      });
    },
  },
};
</script>

<style>
</style>
<template>
  <div>
    <template v-if="selectedValue">
      <app-text-field
        :label="label"
        :value="`${selectedValue.code}-${selectedValue.name} `"
        readonly
        v-bind="$attrs"
        v-on="$listeners"
        @click="open()"
        prepend-inner-icon="mdi-magnify"
        @keypress="open()"
      />
    </template>
    <template v-else>
      <div id="acc" @click.stop="open()">
        <app-text-field
          :label="label"
          readonly
          v-bind="$attrs"
          v-on="$listeners"
          @click="open()"
          prepend-inner-icon="mdi-magnify"
        />
      </div>
    </template>

    <v-dialog
      v-model="dialog"
      max-width="800px"
      scrollable
      :fullscreen="$vuetify.breakpoint.xsOnly"
    >
      <v-card>
        <v-card-title>
          <div class="d-flex justify-space-between" style="width: 100%">
            <div>
              <span>Selecionar {{ label }}</span>
            </div>
          </div>
        </v-card-title>
        <v-card-subtitle>
          <v-text-field
            :loading="isLoading"
            class="mt-4"
            v-model="searchForm.text"
            autofocus
            solo
            flat
            background-color="grey lighten-4"
            prepend-inner-icon="mdi-magnify"
            placeholder="Pesquisar..."
            hide-details
            v-debounce:300ms="handleSearchInput"
          />
        </v-card-subtitle>
        <v-card-text>
          <v-list denses>
            <v-list-item
              v-if="showFinalCustomer"
              @click="setSelectedValue(finalCostumer)"
            >
              <v-list-item-content>
                <v-list-item-title>0 - Consumidor Final</v-list-item-title>
              </v-list-item-content>
            </v-list-item>
            <template v-for="(row, index) in searchResults.data">
              <v-list-item :key="index" @click.stop="setSelectedValue(row)">
                <v-list-item-content>
                  <v-list-item-title>
                    <div class="d-flex align-top">
                      <div>
                        <CustomerLabel :customer="row" />

                        <div>
                          <div>
                            <small
                              class="text--secondary"
                              v-if="row.main_address"
                            >
                              {{ row.main_address.address }}-
                              {{ row.main_address.number }}-
                              {{ row.main_address.city }}
                            </small>
                          </div>
                          <div>
                            <small class="text--secondary" v-if="row.phone_wa">
                              {{ $format.phone(row.phone_wa) }}
                            </small>
                          </div>
                        </div>
                      </div>
                    </div>
                  </v-list-item-title>
                  <!-- <v-list-item-subtitle class="d-flex flex-column">
                    
                  </v-list-item-subtitle> -->
                </v-list-item-content>
              </v-list-item>
              <v-divider :key="'div' + index" />
            </template>
          </v-list>
          <app-pagination :data="searchResults" @click="search($event)" />
        </v-card-text>
        <v-card-actions>
          <v-btn class="text-capitalize" text @click="dialog = false">
            Voltar
          </v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>
  </div>
</template>

<script>
import CustomerLabel from "@/components/customer/sections/CustomerLabel.vue";

export default {
  components: { CustomerLabel },
  props: {
    value: {},
    showFinalCustomer: {
      type: Boolean,
      default: false,
    },
  },

  data: () => ({
    label: "Cliente",
    url: "customer/customer",

    searchResults: {
      data: [],
    },

    selectedValue: {
      name: null,
    },

    dialog: false,

    isLoading: false,

    searchForm: {
      text: null,
      status: 1,
    },
    finalCostumer: {
      name: "Consumidor Final",
      code: 0,
      id: "00",
    },
  }),

  watch: {
    value() {
      this.selectedValue = this.value;
    },
  },

  created() {
    this.selectedValue = this.value;
  },

  methods: {
    async open() {
      await this.search();
      this.searchForm.text = null;

      this.dialog = true;
    },

    async search(page) {
      this.isLoading = true;

      this.searchForm.page = page;

      await this.$http.index(this.url, this.searchForm).then((response) => {
        this.searchResults = response.customers;
      });

      this.isLoading = false;
    },

    setSelectedValue(selectedValue) {
      this.selectedValue = selectedValue;

      this.$emit("input", selectedValue);

      this.dialog = false;
    },

    handleSearchInput() {
      this.search();
    },

    clear() {
      this.selectedValue = null;
    },
  },
};
</script>

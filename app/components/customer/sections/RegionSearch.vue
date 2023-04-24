<template>
  <div>
    <template v-if="selectedValue">
      <app-text-field
        :label="label"
        :value="` ${selectedValue.name} `"
        readonly
        v-bind="$attrs"
        v-on="$listeners"
        @click="open()"
        append-outer-icon="mdi-magnify"
        @keypress="open()"
      />
    </template>
    <template v-else>
      <div @click.stop="open()">
        <app-text-field
          :label="label"
          readonly
          v-bind="$attrs"
          v-on="$listeners"
          @click="open()"
          append-outer-icon="mdi-magnify"
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
            append-outer-icon="mdi-magnify"
            placeholder="Pesquisar..."
            hide-details
            v-debounce:300ms="handleSearchInput"
          />
        </v-card-subtitle>
        <v-card-text>
          <v-list denses>
            <template v-for="(row, index) in searchResults.data">
              <v-list-item :key="index" @click.stop="setSelectedValue(row)">
                <v-list-item-content>
                  <v-list-item-title>
                    {{ row.code }} - {{ row.name }}
                  </v-list-item-title>
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
export default {
  props: {
    value: {},
    showFinalCustomer: {
      type: Boolean,
      default: false,
    },
  },

  data: () => ({
    label: "Região",
    url: "region/region",

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
        this.searchResults = response.regions;
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

import { createVuetify } from 'vuetify'
import * as components from 'vuetify/components'
import * as directives from 'vuetify/directives'
// import { md3 } from 'vuetify/blueprints'
// import { aliases, fa } from 'vuetify/iconsets/fa'
// import { mdi } from 'vuetify/iconsets/mdi'


import '@fortawesome/fontawesome-free/css/all.css'
import '@mdi/font/css/materialdesignicons.css'


import { VDataTable } from 'vuetify/labs/VDataTable'

import aliases from '@/data/vuetify/aliases'
import defaults from '@/data/vuetify/defaults'

export default defineNuxtPlugin(nuxtApp => {

  const vuetify = createVuetify({
    directives,
    aliases,
    defaults,
    components: {
      ...components,
      VDataTable,
    },

    // blueprint: md3,

    // icons : {
    //   defaultSet: 'fa',
    //   aliases,
    //   sets: {
    //     fa,
    //     mdi,
    //   }
    // },

    theme: {
      themes: {
        light: {
          dark: false,
          colors: {
            primary: '#3C3ED2',
            success: '#8bc34a',
            backgroud: "#f5f5f5",
            info: '#00ACC1'
          }
        },
      },
    },

  })

  nuxtApp.vueApp.use(vuetify)
})
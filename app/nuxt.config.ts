// https://nuxt.com/docs/api/configuration/nuxt-config
export default defineNuxtConfig({

	ssr: false,

	imports: {
		dirs: ['store/**', 'composables'],
	},
	plugins: [
		{ src: '~/plugins/loading' }
	],
	components: [
		{
			path: "~/components",
			pathPrefix: false,
		},
	],

	modules: [
		[
			"@pinia/nuxt",
			{
				autoImports: ['defineStore', 'acceptHMRUpdate', 'storeToRefs', 'defineEmits'],

			},
		],
		'@vite-pwa/nuxt',
	],

	css: ["vuetify/lib/styles/main.sass",
		'~/assets/style/variables.sass',


	],

	build: {
		transpile: ["vuetify"],
	},

	app: {
		head: {
			link: [
				{
					rel: "stylesheet",
					href: "https://fonts.googleapis.com/css2?family=Montserrat:ital,wght@0,200;0,500;1,400&display=swap",
				},
			],
		},
	},
});

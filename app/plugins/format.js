
import format from '../utils/format'

export default defineNuxtPlugin(() => {
	return {
		provide: {
			format: format
		},
	};
});

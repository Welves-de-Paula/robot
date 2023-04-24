import { useAuthStore } from "../store/auth";

export default defineNuxtRouteMiddleware(async (to, from) => {

  const authStore = useAuthStore();

  await authStore.boot();

  if (authStore.isLoggedIn) {
    return;
  }

  return navigateTo("/login");

});

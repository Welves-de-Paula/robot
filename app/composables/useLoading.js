import { useAppStore } from '~/store/app';

export default () => {

  const appStore = useAppStore();

  function set(isLoading) {
    appStore.setLoading(isLoading);
  }

  function start() {
    set(true);
  }

  function stop() {
    console.log('stop...')
    set(false);
  }

  return {
    set,
    start,
    stop,
  };
}
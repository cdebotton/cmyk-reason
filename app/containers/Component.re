module type Config = {module type t;};

module Make = (Config: Config) => {
  let cache = SimpleCacheProvider.createCache();
  let resource =
    SimpleCacheProvider.createResource(
      path => DynamicImport.(import({j|$path|j}) |> resolve),
      () => "",
    );
};
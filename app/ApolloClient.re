[@bs.val]
external apolloState : ApolloInMemoryCache.inMemoryCacheRestoreData =
  "window.__APOLLO_STATE__";

let link = ApolloLinks.createHttpLink(~uri="/graphql", ());
let cache =
  ApolloInMemoryCache.(createInMemoryCache() |> restore(apolloState));

let instance = ReasonApollo.createApolloClient(~link, ~cache, ());
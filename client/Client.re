[@bs.val]
external apolloState: ApolloInMemoryCache.inMemoryCacheRestoreData =
  "window.__APOLLO_STATE__";

let link =
  ApolloLinks.createHttpLink(~uri="/graphql", ~credentials="include", ());
let cache =
  ApolloInMemoryCache.(createInMemoryCache() |> restore(apolloState));

let client = ReasonApollo.createApolloClient(~link, ~cache, ());

ReactDOMRe.hydrateToElementWithId(
  <ApolloProvider client>
    <Router.Browser> <App /> </Router.Browser>
  </ApolloProvider>,
  "app",
);

[%bs.raw {| module.hot && module.hot.accept() |}];
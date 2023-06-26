type dataObject = {"__typename": string, "id": string}
// createInMemoryCache(~dataIdFromObject=(obj: dataObject) => obj##id, ());

/* Create an InMemoryCache */
// let inMemoryCache = () => ApolloInMemoryCache.createInMemoryCache();

/* Create an HTTP Link */
let httpLink = (~uri) => ApolloClient.Link.HttpLink.make(~uri=_ => uri, ())

type context =
  | @dead("context.Neither") Neither
  | MaticQuery
  | MainnetQuery
@dead("+chainContextToStr")
let chainContextToStr = chain =>
  switch chain {
  | Neither => "neither"
  | MaticQuery => "matic"
  | MainnetQuery => "mainnet"
  }
type queryContext = {context: option<context>}

@send
external getContext: ApolloClient__Link_Core_ApolloLink.Operation.Js_.t => option<queryContext> =
  "getContext"

/* based on test, execute left or right */
let networkSwitcherHttpLink = (~uri, ~matic, ~mainnet) =>
  ApolloClient.Link.split(
    ~test=operation => {
      let context = operation->getContext

      let dbQuery = switch context {
      | Some({context: Some(actualContext)}) =>
        switch actualContext {
        | MaticQuery => false
        | Neither => true
        | MainnetQuery => false
        }
      | Some(_)
      | None => true
      }
      dbQuery
    },
    ~whenTrue=httpLink(~uri),
    ~whenFalse=ApolloClient.Link.split(~test=operation => {
      let context = operation->getContext

      let usingMatic = switch context {
      | Some({context: Some(actualContext)}) =>
        switch actualContext {
        | MaticQuery => true
        | Neither => false
        | MainnetQuery => false
        }
      | Some(_)
      | None => false
      }
      usingMatic
    }, ~whenTrue=httpLink(~uri=matic), ~whenFalse=httpLink(~uri=mainnet)),
  )

type qlEndpoints = {
  mainnet: string,
  matic: string,
  db: string,
}

let instance = (~getGraphEndpoints: unit => qlEndpoints) => {
  let {mainnet, matic, db} = getGraphEndpoints()

  open ApolloClient
  make(
    ~cache=Cache.InMemoryCache.make(),
    ~connectToDevTools=true,
    ~defaultOptions=DefaultOptions.make(
      ~mutate=DefaultMutateOptions.make(~awaitRefetchQueries=true, ~errorPolicy=All, ()),
      ~query=DefaultQueryOptions.make(
        // ~fetchPolicy=NetworkOnly,
        ~errorPolicy=All,
        (),
      ),
      ~watchQuery=DefaultWatchQueryOptions.make(
        // ~fetchPolicy=NetworkOnly,
        ~errorPolicy=All,
        (),
      ),
      (),
    ),
    ~link=networkSwitcherHttpLink(~uri=db, ~mainnet, ~matic),
    (),
  )
}

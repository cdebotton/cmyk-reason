import React from 'react';
import Koa, { Context } from 'koa';
import https from 'https';
import path from 'path';
import fs from 'fs';
import { ApolloServer } from 'apollo-server-koa';
import { InMemoryCache } from 'apollo-cache-inmemory';
import { SchemaLink } from 'apollo-link-schema';
import { ApolloClient } from 'apollo-client';
import { ApolloProvider, getDataFromTree } from 'react-apollo';
import { Prisma } from 'prisma-binding';
import render from './middleware/render';
// @ts-ignore
import ServerApp from '../lib/js/app/ServerApp.bs.js';
// @ts-ignore
import { staticRouterJs as StaticRouter } from '../lib/js/app/containers/Router.bs.js';
import schema from './api/schema';

const { PORT = '3000' } = process.env;

const app = new Koa();
const db = new Prisma({
  typeDefs: './server/api/schema.graphql',
  endpoint: 'http://localhost:4466',
});
const apollo = new ApolloServer({
  schema,
  context: ({ ctx }: { ctx: Context }) => ({
    ...ctx,
    db,
  }),
});

const ssl = path.join(process.cwd(), '.ssl');

const server = https.createServer(
  {
    cert: fs.readFileSync(path.join(ssl, 'localhost.crt')),
    key: fs.readFileSync(path.join(ssl, 'localhost.key')),
  },
  app.callback(),
);

// @ts-ignore
apollo.applyMiddleware({ app });

app.use(
  render(async ctx => {
    const link = new SchemaLink({
      schema,
      context: () => ({
        ...ctx,
        db,
      }),
    });

    const cache = new InMemoryCache();
    const client = new ApolloClient({ link, cache });
    const context: any = {};
    const app = (
      <ApolloProvider client={client}>
        <StaticRouter pathname={ctx.req.url} context={context}>
          <ServerApp />
        </StaticRouter>
      </ApolloProvider>
    );

    await getDataFromTree(app);
    const [statusCode, redirect] = context;
    const state = JSON.stringify(client.extract());

    return [app, state, statusCode, redirect];
  }),
);

server.listen(PORT, () => {
  process.stdout.write(`🚀 Running on port ${PORT}\n`);
});

if (process.env.NODE_ENV === 'development') {
  app.listen(4000);
}

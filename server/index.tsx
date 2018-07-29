import React from 'react';
import Koa, { Context } from 'koa';
import mount from 'koa-mount';
import serveStatic from 'koa-static';
import compress from 'koa-compress';
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

const { PORT = '3000', NODE_ENV = 'development' } = process.env;

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

app.use(compress());

if (NODE_ENV !== 'development') {
  app.use(mount('/dist', serveStatic(path.join(process.cwd(), 'dist/client'))));
}

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
    const context = {
      url: undefined,
      statusCode: 200,
    };

    const app = (
      <ApolloProvider client={client}>
        <StaticRouter pathname={ctx.req.url} context={context}>
          <ServerApp />
        </StaticRouter>
      </ApolloProvider>
    );

    await getDataFromTree(app);

    const { statusCode, url } = context;
    const state = JSON.stringify(client.extract());

    return [app, state, statusCode, url];
  }),
);

server.listen(PORT, () => {
  process.stdout.write(`🚀 Running on port ${PORT}\n`);
});

if (NODE_ENV === 'development') {
  app.listen(4000);
}

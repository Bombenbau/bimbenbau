Deno.serve((req: Request) => {
	let path = new URL(req.url).pathname;
	path === "/" && (path = "/index");

	switch (path) {
		case "/on":
                  console.log(`update: ${req.url}`);
                  return new Response("update");

            default:
                  console.log(`${path}`);
                  return new Response("404", {status: 404});
	}
});


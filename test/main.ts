const voltage = 0.5;

const wattage: Record<string, number> = {
	optimized: 0,
	unoptimized: 0,
};

const calculateWattage = (wattage: number) => wattage * voltage;

Deno.serve((req: Request) => {
	let path = new URL(req.url).pathname;
	path === "/" && (path = "/index.html");

	// Input current GET /optimized/0.00/unoptimized/0.00

	const regex = /\/optimized\/(\d+\.\d+)\/unoptimized\/(\d+\.\d+)/;

	if (req.method === "GET" && regex.test(path)) {
		const [, optimized, unoptimized] = path.match(regex) as RegExpMatchArray;

		// Calculate wattage
		wattage.optimized = calculateWattage(parseFloat(optimized));
		wattage.unoptimized = calculateWattage(parseFloat(unoptimized));

		console.log(wattage);

		return new Response("");
	} else if (req.method === "GET" && path === "/data") {
		return new Response(JSON.stringify(wattage), {status: 200, headers: {"Content-Type": "application/json"}});
	} else if (req.method === "GET") {
		// Try to find the file if present
		try {
			return new Response(Deno.readTextFileSync(`.${path}`), {status: 200, headers: {"Content-Type": "text/html"}});
		} catch {
			return new Response("Not found", {status: 404});
		}
	} else {
		return new Response("Not found", {status: 404});
	}
});

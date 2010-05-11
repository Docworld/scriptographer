include('Raster.js');

function createDot(x, y, dot, radius) {
	if (radius > 0.1) {
		var item = dot.clone();
		item.position += new Point(x, y) * values.size;
		item.scale(radius * values.scale);
		return item;
	}
}

if (initRaster()) {
	var components = {
		size: { value: 10, label: 'Grid Size'},
		scale: { value: 100, label: 'Object Scale (%)'}
	};
	var values = Dialog.prompt('Enter Raster Values:', components);
	if (values) {
		values.scale /= 100;
		executeRaster(createDot);
	}
}
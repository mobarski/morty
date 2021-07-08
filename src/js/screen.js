function init_screen(width=800, height=400) {	
	var screen = document.getElementById("screen")
	screen.innerHTML = `<canvas id="main_canvas" width="${width}" height="${height}""></canvas>`
	
	out = document.getElementById("output")
	cnv = document.getElementById("main_canvas")
	
	ctx = cnv.getContext("2d")
	ctx.imageSmoothingEnabled = false
}

function fullscreen() {
	var elem = cnv
	if (elem.requestFullscreen) {
		elem.requestFullscreen()
	} else if (elem.mozRequestFullScreen) { /* Firefox */
		elem.mozRequestFullScreen()
	} else if (elem.webkitRequestFullscreen) { /* Chrome, Safari and Opera */
		elem.webkitRequestFullscreen()
	} else if (elem.msRequestFullscreen) { /* IE/Edge */
		elem.msRequestFullscreen()
	}
}

function color(r,g,b,a=1) {
	ctx.fillStyle = `rgb(${r},${g},${b},${a})`
	ctx.strokeStyle = ctx.fillStyle
}

function rect(x,y,w,h) {
	ctx.fillRect(x,y,w,h)
}

function cls() {
	rect(0, 0, ctx.canvas.width, ctx.canvas.height)
}

function pset(x,y) {
	rect(x,y,1,1)
}

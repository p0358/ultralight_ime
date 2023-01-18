#pragma once

const char* htmlString()
{
	return R"(
<html>
	<head>
		<title>IME Test</title>
		<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />
		<style>
			h2 {
				font-size: 2rem;
				font-weight: 600;
				line-height: 1.125;
				margin-top: 15px;
			}

			html {
				background: #2a2b36;
				font-size: 11pt;
				color: #cacad8;
			}

			html, body {
				margin: 0;
				padding: 0;
			}

			.maincontainer {
				padding: 20px 20px 0 20px;
				padding: 0em 3em;
			}

			body, button, input, select, textarea {
				font-family: BlinkMacSystemFont, -apple-system, 'Segoe UI', 
										 'Roboto', 'Ubuntu', 'Helvetica Neue', 'Helvetica', 'Arial', sans-serif;
			}

			.status {
				margin-bottom: 30px;
				padding: 1em;
				background: rgb(2 16 5 / 36%);
				border-radius: 8px;
				box-shadow: 0 0.5em 1em -0.125em rgb(10 10 10 / 30%), 0 0 0 1px rgb(10 10 10 / 2%);
			}

			input {
				width: calc(100% - .4em); 
				padding: 8px;
			}

			.panel {
				border-radius: 8px;
				box-shadow: 0 0.5em 1em -0.125em rgb(10 10 10 / 30%), 0 0 0 1px rgb(10 10 10 / 2%);
				font-size: 1rem;
				border: 1px solid #4d0a9b;
				border: 1px solid #181921;
				overflow: hidden;
			}

			.panel-block {
				color: #fff;
				padding: 0.0em 0.75em;
				background: linear-gradient(180deg, #00000038, transparent);
				padding: 1em 1em;
				display: flex;
			}

			.panel-block div:nth-child(1) {
				width: 100%;
			}
			
			.panel-block div:nth-child(2) {
				width: 150px;
				flex: 0 0 150px;
			}

			.panel-block p {
				display: block;
				margin: 0;
				padding: 0.25em;
			}

			.panel-block:not(:last-child), .panel-tabs:not(:last-child) {
				border-bottom: 1px solid #181921;
			}

			ol {
				list-style-type: decimal;
				display: flex;
				flex-wrap: wrap;
				background: rgb(2 16 5 / 36%);
				border-radius: 4px;
				box-shadow: 0 0.5em 1em -0.125em rgb(10 10 10 / 30%), 0 0 0 1px rgb(10 10 10 / 2%);
				font-size: 1rem; /*1.1rem;*/
				position: absolute;
				padding: 0;
				white-space: nowrap;
				overflow: hidden;
				text-overflow: ellipsis;
				flex-wrap: nowrap;
			}

			ol li {
				border-right: 1.5px solid white;
				padding: .2em; /*.6em;*/
				padding-left: 0;
				box-sizing: border-box;
				margin-left: 30px;
				padding-right: 10px; /*15px;*/
			}

			ol li:first-child {
				margin-left: 30px;
			}

			ol li:last-child {
				margin-right: 0;
				border-right: none;
			}

			/* does not work in WebKit/Ultralight it seems, so we are not using this for now... */
			/*body.ime-composing input::selection, body.ime-composing textarea::selection {
				background-color: transparent !important;
				color: black !important;
				text-decoration: underline 2px solid black !important;
				-webkit-text-decoration-line: underline !important;
				-webkit-text-decoration-color: black !important;
				-webkit-text-decoration-style: solid !important;
				-webkit-text-decoration-thickness: 2px !important;
			}*/
		</style>
	</head>
	<body>

		<ol id="candidates" style="display: none">
			<li>test1</li>
			<li>test2</li>
			<li>test3test3test3test3</li>
			<li>test4</li>
			<li>test5</li>
			<li>test6</li>
			<li>test7</li>
			<li>test8</li>
		</ol>

		<div class="maincontainer">

			<h2>IME Test</h2>

			<div class="status">
				Below you can type stuff in Chinese, Korean and Japanese. Go try!
			</div>

			<div id="targets">
				<nav class="panel">
					<div class="panel-block">
						<div>
							<p><input type="text" value="" id="text" /></p>
						</div>
					</div>
				</nav>
			</div>

		</div>

		<script>
			function getPosition(n, endNode) {
				let left = 0;
				let top = 0;
				let node = n;
				let done = false;
				while (!done) {
					if (node.offsetLeft !== null)
						left += node.offsetLeft;
					if (node.offsetTop !== null)
						top += node.offsetTop;
					if (node.offsetParent)
						node = node.offsetParent;
					else
						done = true;
					if (node === endNode)
						done = true;
				}
				done = false;
				node = n;
				while (!done) {
					if (document.all && node.style && parseInt(node.style.borderLeftWidth))
						left += parseInt(node.style.borderLeftWidth);

					if (document.all && node.style && parseInt(node.style.borderTopWidth))
						top += parseInt(node.style.borderTopWidth);

					if (node.scrollLeft)
						left -= node.scrollLeft;
					if (node.scrollTop)
						top -= node.scrollTop;
					if (node.parentNode)
						node = node.parentNode;
					else
						done = true;
				}
				return [left, top];
			}

			const candidates = document.getElementById("candidates");

			const CompositionState_StartComposition = 0;
			const CompositionState_Composing = 1;
			const CompositionState_Commit = 2;
			const CompositionState_EndComposition = 3;

			window._nativeEventCallback = function (name, ...args) {
				console.log("[" + arguments.length + "] _nativeEventCallback: " + [...arguments].map(a => a+'').join(";"));
				
				const active = document.activeElement;
				if (!active || !("value" in active) || !("selectionStart" in active) || !("selectionEnd" in active))
					return;

				if (name === "IME_OnCandidateList") {
					candidates.innerHTML = "";
					const newCands = args[0];
					for (const cand of newCands) {
						const candel = document.createElement("li");
						candel.innerText = cand;
						candidates.appendChild(candel);
					}
					candidates.style.display = newCands.length ? "flex" : "none";

					var pos = getPosition(active);
					candidates.style.left = pos[0] + "px";
					candidates.style.top = (pos[1] + active.offsetHeight) + "px";
				}

				if (name === "IME_OnComposition") {
					const compositionState = args[0];
					const str = args[1];

					switch (compositionState) {

					case CompositionState_Composing: {
						const selStart = active.selectionStart;
						active.value = active.value.substring(0, active.selectionStart)
							+ str + active.value.substring(active.selectionEnd);
						active.selectionStart = selStart;
						active.selectionEnd = selStart + str.length;

						if (str.length)
							document.body.classList.add("ime-composing");
						else
							document.body.classList.remove("ime-composing");

						break;
					}

					case CompositionState_Commit: {
						const selStart = active.selectionStart;
						active.value = active.value.substring(0, active.selectionStart)
							+ str + active.value.substring(active.selectionEnd);
						active.selectionStart = selStart + str.length;
						active.selectionEnd = active.selectionStart;
						document.body.classList.remove("ime-composing");
						break;
					}

					case CompositionState_EndComposition:
						document.body.classList.remove("ime-composing");
						break;

					}
				}

			};
		</script>
	</body>
</html>
)";
}

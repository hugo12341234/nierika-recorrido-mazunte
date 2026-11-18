// Convierte portada.html y cuerpo.html (carpeta dada) en portada.pdf y cuerpo.pdf con Chromium (Playwright).
const { chromium } = require('playwright');
const path = require('path');
(async () => {
  const dir = process.argv[2], pie = process.argv[3] || '';
  const b = await chromium.launch({ args: ['--no-sandbox'] });
  const p = await b.newPage();
  await p.goto('file://' + path.join(dir, 'portada.html'));
  await p.pdf({ path: path.join(dir, 'portada.pdf'), format: 'A4', printBackground: true, margin: { top: '0', bottom: '0', left: '0', right: '0' } });
  await p.goto('file://' + path.join(dir, 'cuerpo.html'), { waitUntil: 'load' });
  await p.evaluate(() => Promise.all(Array.from(document.images).map(i => i.complete ? 1 : new Promise(r => { i.onload = i.onerror = r; }))));
  const esc = s => s.replace(/&/g, '&amp;').replace(/</g, '&lt;');
  await p.pdf({
    path: path.join(dir, 'cuerpo.pdf'), format: 'A4', printBackground: true, outline: true, tagged: true,
    displayHeaderFooter: true,
    headerTemplate: '<div style="width:100%;font-size:7.5px;font-family:Inter,Arial;color:#8a6d3b;padding:0 18mm;text-align:right">NIERIKA STUDIO</div>',
    footerTemplate: '<div style="width:100%;font-size:7.5px;font-family:Inter,Arial;color:#666;padding:0 18mm;display:flex;justify-content:space-between"><span>' + esc(pie) + '</span><span>Página <span class="pageNumber"></span> de <span class="totalPages"></span></span></div>',
    margin: { top: '20mm', bottom: '18mm', left: '18mm', right: '18mm' }
  });
  await b.close();
})().catch(e => { console.error(e); process.exit(1); });

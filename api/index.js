import express from 'express';
import { chdir } from 'process';
import { spawn } from 'child_process';
import fs from 'fs';
import cors from 'cors';

const app = express();
app.use(express.json());
app.use(express.static('../frontend/build'));
app.use(cors());

chdir('../build-dir');
const proc = spawn('./goose');
proc.stdin.write('load\n');
let rid = 0;

app.post('/api/query', async (req, res) => {
  const { terms, options } = req.body;
  proc.stdin.write(`query ${options.join(' ')} ${terms.join(' ')}\n`);
  const fileUrl = new URL(
    `../build-dir/query-res/res${rid++}`,
    import.meta.url
  );
  while (!fs.existsSync(fileUrl)) {
    await sleep(100);
  }
  const content = fs.readFileSync(fileUrl).toString();
  const lines = content.split('\n');
  let results = [];
  for (let i = 0; i < lines.length - 1; i += 2) {
    results.push({
      url: lines[i],
      snippet: lines[i + 1],
    });
  }
  res.send(results);
});

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

app.listen(5000);

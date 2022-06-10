import { Link } from 'react-router-dom';

import './styles.css';

const HomeGooseLogo = () => (
  <img className="home-logo" src="/goose-logo.png" alt="Goose Logo" />
);

const DocumentLogo = () => (
  <a href="http://lyang.tech/projects/goose">
    <img src="/document-logo.png" alt="Document Logo" />
  </a>
);

const GithubLogo = () => (
  <a href="https://github.com/ileoyang/goose-search-engine">
    <img src="/github-logo.png" alt="GitHub Logo" />
  </a>
);

const SearchGooseLogo = () => (
  <Link to="/">
    <img className="search-logo" src="/goose-logo.png" alt="Goose Logo" />
  </Link>
);

export { HomeGooseLogo, DocumentLogo, GithubLogo, SearchGooseLogo };

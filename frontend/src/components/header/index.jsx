import './styles.css';
import { DocumentLogo, GithubLogo } from '../logo';
import { SearchGooseLogo } from '../logo';
import { SearchBar } from '../search-bar';

const HomeHeader = () => (
  <div className="home-header">
    <DocumentLogo />
    <GithubLogo />
  </div>
);

const SearchHeader = () => (
  <div className="search-header">
    <SearchGooseLogo />
    <SearchBar hasButton={false} />
  </div>
);

export { HomeHeader, SearchHeader };

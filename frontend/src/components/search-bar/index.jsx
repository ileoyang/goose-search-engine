import { useHistory } from 'react-router';
import { useRef } from 'react';

import './styles.css';
import { useQuery } from '../../hooks/use-query';

const SearchBar = ({ hasButton = true }) => {
  const history = useHistory();
  const searchInputRef = useRef();
  const query = useQuery();

  const search = (e) => {
    e.preventDefault();
    history.push('/search?terms=' + searchInputRef.current.value);
  };

  return (
    <>
      <div className="search-bar">
        <form>
          <div className="search-box">
            <input
              className="search-input"
              ref={searchInputRef}
              defaultValue={query.get('terms')}
            />
          </div>
          <div className="search-button">
            <input
              type="submit"
              onClick={search}
              value="Goose Search"
              hidden={!hasButton}
            />
          </div>
        </form>
      </div>
    </>
  );
};

export { SearchBar };

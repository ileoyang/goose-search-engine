import { useEffect, useState } from 'react';
import axios from 'axios';
import Ansi from 'ansi-to-react';

import './styles.css';
import { useQuery } from '../../hooks/use-query';

const SearchList = () => {
  const [results, setResults] = useState([]);
  const query = useQuery();

  useEffect(() => {
    getResults();
  }, [query]);

  const getResults = async () => {
    try {
      const terms = query.get('terms').split(' ');
      const res = await axios.get(
        `/api/search?terms=${terms.join(',')}&options=con`
      );
      setResults(res.data);
    } catch (error) {
      console.log(error);
    }
  };

  return (
    <div className="search-list">
      {results.map((result, i) => (
        <div className="search-result" key={i}>
          <a className="search-link" href={result.url}>
            {result.url}
          </a>
          <a className="search-title" href={result.url}>
            <h3>Title {i + 1}</h3>
          </a>
          <Ansi className="search-snippet">{result.snippet}</Ansi>
        </div>
      ))}
    </div>
  );
};

export { SearchList };
